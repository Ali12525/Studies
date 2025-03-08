using ShopApp.Services;
using ShopApp.Models;
using Moq;

namespace ShopAppTests.Controllers
{
    [TestFixture]
    public class TestsProductController
    {
        private ProductsController _controller;
        private Mock<IProductService> _mockProductService;
        private Product _testProduct;

        [SetUp]
        public void Setup()
        {
            _mockProductService = new Mock<IProductService>();
            _controller = new ProductsController(_mockProductService.Object);

            _testProduct = new Product
            {
                Id = Guid.NewGuid(),
                Description = "Test Product",
                Price = 10.0
            };
        }

        // Tests for CreateProduct
        [Test]
        public void CreateProduct_ShouldReturnProduct_WhenValidInput()
        {
            _mockProductService.Setup(x => x.Add(It.IsAny<Product>())).Returns(_testProduct);
            var result = _controller.CreateProduct("Valid Product", 15.99);

            Assert.That(result, Is.EqualTo(_testProduct), "Should return created product");
            _mockProductService.Verify(x => x.Add(It.IsAny<Product>()), Times.Once);
        }

        [TestCase("", 10)]
        [TestCase("Invalid Product", -5)]
        public void CreateProduct_ShouldReturnNull_WhenInvalidInput(string description, double price)
        {
            var result = _controller.CreateProduct(description, price);

            Assert.That(result, Is.Null, "Should return null for invalid input");
            _mockProductService.Verify(x => x.Add(It.IsAny<Product>()), Times.Never);
        }

        // Tests for SearchProduct
        [Test]
        public void SearchProduct_ShouldReturnProduct_WhenExists()
        {
            _mockProductService.Setup(x => x.Search(_testProduct.Id)).Returns(_testProduct);
            var result = _controller.SearchProduct(_testProduct.Id);

            Assert.That(result, Is.EqualTo(_testProduct), "Should return existing product");
            _mockProductService.Verify(x => x.Search(_testProduct.Id), Times.Once);
        }

        [Test]
        public void SearchProduct_ShouldReturnNull_WhenNotExists()
        {
            var nonExistentId = Guid.NewGuid();
            var result = _controller.SearchProduct(nonExistentId);

            Assert.That(result, Is.Null, "Should return null for non-existent product");
            _mockProductService.Verify(x => x.Search(nonExistentId), Times.Once);
        }

        // Tests for RemoveProduct
        [Test]
        public void RemoveProduct_ShouldReturnProduct_WhenExists()
        {
            _mockProductService.Setup(x => x.Remove(_testProduct.Id)).Returns(_testProduct);
            var result = _controller.RemoveProduct(_testProduct.Id);

            Assert.That(result, Is.EqualTo(_testProduct), "Should return removed product");
            _mockProductService.Verify(x => x.Remove(_testProduct.Id), Times.Once);
        }

        [Test]
        public void RemoveProduct_ShouldReturnNull_WhenNotExists()
        {
            var nonExistentId = Guid.NewGuid();
            _mockProductService.Setup(x => x.Remove(nonExistentId)).Returns((Product?)null);
            var result = _controller.RemoveProduct(nonExistentId);

            Assert.That(result, Is.Null, "Should return null for non-existent product");
            _mockProductService.Verify(x => x.Remove(nonExistentId), Times.Once);
        }

        // Tests for EditProduct
        [Test]
        public void EditProduct_ShouldUpdateProduct_WhenValidInput()
        {
            var updatedProduct = new Product
            {
                Id = _testProduct.Id,
                Description = "Updated Product",
                Price = 20.0
            };

            _mockProductService.Setup(x => x.Search(_testProduct.Id)).Returns(_testProduct);
            _mockProductService.Setup(x => x.Edit(It.IsAny<Product>())).Returns(updatedProduct);

            var result = _controller.EditProduct(_testProduct.Id, updatedProduct.Description, updatedProduct.Price);

            Assert.Multiple(() =>
            {
                Assert.That(result, Is.EqualTo(updatedProduct), "Should return updated product");
                Assert.That(result.Description, Is.EqualTo(updatedProduct.Description), "Description should be updated");
                Assert.That(result.Price, Is.EqualTo(updatedProduct.Price), "Price should be updated");
            });

            _mockProductService.Verify(x => x.Edit(It.IsAny<Product>()), Times.Once);
        }

        [TestCase("Invalid Product", -5)]
        [TestCase("", 10)]
        public void EditProduct_ShouldReturnNull_WhenInvalidInput(string description, double price)
        {
            _mockProductService.Setup(x => x.Search(_testProduct.Id)).Returns(_testProduct);
            var result = _controller.EditProduct(_testProduct.Id, description, price);

            Assert.That(result, Is.Null, "Should return null for invalid input");
            _mockProductService.Verify(x => x.Edit(It.IsAny<Product>()), Times.Never);
        }

        [Test]
        public void EditProduct_ShouldReturnNull_WhenProductNotExists()
        {
            var nonExistentId = Guid.NewGuid();
            var result = _controller.EditProduct(nonExistentId, "New Description", 15.0);

            Assert.That(result, Is.Null, "Should return null for non-existent product");
            _mockProductService.Verify(x => x.Edit(It.IsAny<Product>()), Times.Never);
        }
    }
}