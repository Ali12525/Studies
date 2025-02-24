using Microsoft.AspNetCore.Mvc;
using ShopApp.Models;

// <summary>
// Provides endpoints for interacting with products
// </summary>
[Route("api/[controller]/[action]")]
[ApiController]
public class ProductsController : ControllerBase
{
    private Dictionary<Guid, Product> _products;

    public ProductsController()
    {
        _products = new Dictionary<Guid, Product>();
        this.InitProducts();
    }

    private void InitProducts()
    {
        foreach (var item in Enumerable.Range(8, 180))
        {
            var product = new Product()
            {
                Description = $"Information about {item}",
                Id = Guid.NewGuid(),
                Price = Random.Shared.NextDouble()
            };
            _products.Add(product.Id, product);
        }
    }

    /// <summary>
    /// Get Products
    /// </summary>
    /// <returns>Json array</returns>
    [HttpGet]
    public IEnumerable<Product> GetProducts(int start = 0, int take = 10)
    {
        return this._products.Skip(start).Take(take).Select(x => x.Value).ToArray();
    }

    /// <summary>
    /// Creates a product with the specified description
    /// </summary>
    /// <param name="description">The description</param>
    /// <param name="price">The price of the product</param>
    /// <returns>Returns the product ID if the product has been successfully added to the database. Otherwise, it returns an empty ID.</returns>
    [HttpPost]
    public Guid CreateProduct(string description, double price)
    {
        if (string.IsNullOrEmpty(description) || price < 0)
        {
            return Guid.Empty;
        }
        var product = new Product()
        {
            Id = Guid.NewGuid(),
            Description = description,
            Price = price
        };
        if (this._products.TryAdd(product.Id, product))
        {
            return product.Id;
        }
        return Guid.Empty;
    }
    
    /// <summary>
    /// Get Information about product by id
    /// </summary>
    /// <param name="id">The ID of the product</param>
    /// <returns>Returns product if the product exists. Otherwise, it returns null</returns>
    [HttpGet]
    public Product? GetProduct(Guid id)
    {
        if (this._products.ContainsKey(id)) return this._products[id];
        return null;
    }
}   