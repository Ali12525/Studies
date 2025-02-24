using Microsoft.AspNetCore.Mvc;
using ShopApp.Models;

// <summary>
// Provides endpoints for interacting with products
// </summary>
[Route("api/[controller]")]
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
    [Route("List")]
    public IEnumerable<Product> GetProducts(int start = 0, int take = 10)
    {
        return this._products.Skip(start).Take(take).Select(x => x.Value).ToArray();
    }
}